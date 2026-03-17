#include <gtest/gtest.h>
#include <sstream>
#include "ChainingHashTable.h"
#include "ProbingHashTable.h"

// Helper: capture print() output for ChainingHashTable
static std::string capture_chain_print(const ChainingHashTable& table) {
    std::ostringstream oss;
    auto* old_buf = std::cout.rdbuf(oss.rdbuf());
    table.print();
    std::cout.rdbuf(old_buf);
    return oss.str();
}

// Helper: capture print() output for ProbingHashTable
static std::string capture_probe_print(const ProbingHashTable& table) {
    std::ostringstream oss;
    auto* old_buf = std::cout.rdbuf(oss.rdbuf());
    table.print();
    std::cout.rdbuf(old_buf);
    return oss.str();
}

// =============================================================================
// CHAINING HASH TABLE TESTS (TODOs 1-8, 75 pts)
// =============================================================================

// ==================== TODO 1: Constructor (5 pts) ============================

TEST(ChainingTest, ConstructorDefaultCapacity) {
    ChainingHashTable table;
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.capacity(), 7);
    EXPECT_TRUE(table.is_empty());
}

TEST(ChainingTest, ConstructorCustomCapacity) {
    ChainingHashTable table(17);
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.capacity(), 17);
}

// ==================== TODO 2: Destructor (5 pts) =============================

TEST(ChainingTest, DestructorCleansUpMemory) {
    // If the destructor is broken, sanitizers will catch the leak.
    auto* table = new ChainingHashTable();
    table->insert("alice", 90);
    table->insert("bob", 85);
    table->insert("carol", 78);
    delete table;
}

TEST(ChainingTest, DestructorEmptyTable) {
    auto* table = new ChainingHashTable();
    delete table;
}

// ==================== TODO 3: hash() (10 pts) ================================

TEST(ChainingTest, HashConsistentSameKey) {
    ChainingHashTable table;
    size_t h1 = table.hash("test");
    size_t h2 = table.hash("test");
    EXPECT_EQ(h1, h2);
}

TEST(ChainingTest, HashWithinBounds) {
    ChainingHashTable table;
    EXPECT_LT(table.hash("alice"), 7u);
    EXPECT_LT(table.hash("bob"), 7u);
    EXPECT_LT(table.hash("superlongkeyname"), 7u);
}

TEST(ChainingTest, HashDifferentKeys) {
    ChainingHashTable table;
    // Different keys should (usually) produce different hashes.
    // Not guaranteed, but "alice" and "bob" are very likely different.
    size_t h1 = table.hash("alice");
    size_t h2 = table.hash("bob");
    EXPECT_NE(h1, h2);
}

// ==================== TODO 4: insert() (10 pts) ==============================

TEST(ChainingTest, InsertSingleEntry) {
    ChainingHashTable table;
    table.insert("alice", 90);
    EXPECT_EQ(table.size(), 1);
    EXPECT_FALSE(table.is_empty());
}

TEST(ChainingTest, InsertMultipleEntries) {
    ChainingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    EXPECT_EQ(table.size(), 3);
}

TEST(ChainingTest, InsertDuplicateKeyUpdatesValue) {
    ChainingHashTable table;
    table.insert("alice", 90);
    table.insert("alice", 95);
    EXPECT_EQ(table.size(), 1);
    int* val = table.search("alice");
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 95);
}

// ==================== TODO 5: search() (10 pts) ==============================

TEST(ChainingTest, SearchFindsExistingKey) {
    ChainingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    int* result = table.search("alice");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 90);
}

TEST(ChainingTest, SearchReturnsNullForMissingKey) {
    ChainingHashTable table;
    table.insert("alice", 90);
    EXPECT_EQ(table.search("bob"), nullptr);
}

TEST(ChainingTest, SearchEmptyTable) {
    ChainingHashTable table;
    EXPECT_EQ(table.search("alice"), nullptr);
}

TEST(ChainingTest, SearchAfterUpdate) {
    ChainingHashTable table;
    table.insert("alice", 90);
    table.insert("alice", 100);
    int* result = table.search("alice");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 100);
}

// ==================== TODO 6: remove() (10 pts) ==============================

TEST(ChainingTest, RemoveExistingKey) {
    ChainingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    EXPECT_TRUE(table.remove("alice"));
    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.search("alice"), nullptr);
}

TEST(ChainingTest, RemoveMissingKeyReturnsFalse) {
    ChainingHashTable table;
    table.insert("alice", 90);
    EXPECT_FALSE(table.remove("bob"));
    EXPECT_EQ(table.size(), 1);
}

TEST(ChainingTest, RemoveFromEmptyTable) {
    ChainingHashTable table;
    EXPECT_FALSE(table.remove("alice"));
}

TEST(ChainingTest, RemoveHeadOfChain) {
    // Insert keys that collide, then remove the head
    ChainingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    int original_size = table.size();
    // Remove one and verify the others remain
    table.remove("alice");
    EXPECT_EQ(table.size(), original_size - 1);
    EXPECT_EQ(table.search("alice"), nullptr);
    ASSERT_NE(table.search("bob"), nullptr);
    ASSERT_NE(table.search("carol"), nullptr);
}

TEST(ChainingTest, RemoveOnlyElement) {
    ChainingHashTable table;
    table.insert("alice", 90);
    EXPECT_TRUE(table.remove("alice"));
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.is_empty());
}

// ==================== TODO 7: load_factor() / resize() (15 pts) ==============

TEST(ChainingTest, LoadFactorEmpty) {
    ChainingHashTable table;
    EXPECT_DOUBLE_EQ(table.load_factor(), 0.0);
}

TEST(ChainingTest, LoadFactorAfterInserts) {
    ChainingHashTable table;  // capacity 7
    table.insert("a", 1);
    table.insert("b", 2);
    table.insert("c", 3);
    // 3 / 7 ≈ 0.4286
    EXPECT_NEAR(table.load_factor(), 3.0 / 7.0, 0.01);
}

TEST(ChainingTest, ResizeTriggersOnHighLoadFactor) {
    ChainingHashTable table;  // capacity 7
    // Insert enough entries to exceed load factor of 1.0
    table.insert("a", 1);
    table.insert("b", 2);
    table.insert("c", 3);
    table.insert("d", 4);
    table.insert("e", 5);
    table.insert("f", 6);
    table.insert("g", 7);
    table.insert("h", 8);  // 8/7 > 1.0, triggers resize
    EXPECT_GT(table.capacity(), 7);
    EXPECT_EQ(table.size(), 8);
}

TEST(ChainingTest, ResizePreservesAllEntries) {
    ChainingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    table.insert("dave", 92);
    table.insert("eve", 88);
    table.insert("frank", 76);
    table.insert("grace", 95);
    table.insert("heidi", 81);  // triggers resize

    // All entries should still be searchable
    ASSERT_NE(table.search("alice"), nullptr);
    EXPECT_EQ(*table.search("alice"), 90);
    ASSERT_NE(table.search("bob"), nullptr);
    EXPECT_EQ(*table.search("bob"), 85);
    ASSERT_NE(table.search("heidi"), nullptr);
    EXPECT_EQ(*table.search("heidi"), 81);
}

// ==================== TODO 8: print() (10 pts) ===============================

TEST(ChainingTest, PrintEmptyTable) {
    ChainingHashTable table;
    std::string output = capture_chain_print(table);
    // Every bucket should say "empty"
    EXPECT_NE(output.find("empty"), std::string::npos);
}

TEST(ChainingTest, PrintShowsEntries) {
    ChainingHashTable table;
    table.insert("alice", 90);
    std::string output = capture_chain_print(table);
    EXPECT_NE(output.find("alice"), std::string::npos);
    EXPECT_NE(output.find("90"), std::string::npos);
}

TEST(ChainingTest, PrintFormatBucketIndex) {
    ChainingHashTable table;
    std::string output = capture_chain_print(table);
    // Should contain bucket indices like "  [0]: "
    EXPECT_NE(output.find("  [0]: "), std::string::npos);
    EXPECT_NE(output.find("  [6]: "), std::string::npos);
}

// =============================================================================
// PROBING HASH TABLE TESTS (TODOs 9-13, 55 pts)
// =============================================================================

// ==================== TODO 9: Constructor (5 pts — provided) =================

TEST(ProbingTest, ConstructorDefaultCapacity) {
    ProbingHashTable table;
    EXPECT_EQ(table.size(), 0);
    EXPECT_EQ(table.capacity(), 7);
    EXPECT_TRUE(table.is_empty());
}

// ==================== TODO 10: insert() (10 pts) =============================

TEST(ProbingTest, InsertSingleEntry) {
    ProbingHashTable table;
    table.insert("alice", 90);
    EXPECT_EQ(table.size(), 1);
    EXPECT_FALSE(table.is_empty());
}

TEST(ProbingTest, InsertMultipleEntries) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    EXPECT_EQ(table.size(), 3);
}

TEST(ProbingTest, InsertDuplicateKeyUpdatesValue) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("alice", 95);
    EXPECT_EQ(table.size(), 1);
    int* val = table.search("alice");
    ASSERT_NE(val, nullptr);
    EXPECT_EQ(*val, 95);
}

TEST(ProbingTest, InsertHandlesCollision) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    table.insert("dave", 92);
    // All four should be findable despite potential collisions
    ASSERT_NE(table.search("alice"), nullptr);
    ASSERT_NE(table.search("bob"), nullptr);
    ASSERT_NE(table.search("carol"), nullptr);
    ASSERT_NE(table.search("dave"), nullptr);
}

// ==================== TODO 11: search() (10 pts) =============================

TEST(ProbingTest, SearchFindsExistingKey) {
    ProbingHashTable table;
    table.insert("alice", 90);
    int* result = table.search("alice");
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(*result, 90);
}

TEST(ProbingTest, SearchReturnsNullForMissingKey) {
    ProbingHashTable table;
    table.insert("alice", 90);
    EXPECT_EQ(table.search("bob"), nullptr);
}

TEST(ProbingTest, SearchEmptyTable) {
    ProbingHashTable table;
    EXPECT_EQ(table.search("alice"), nullptr);
}

TEST(ProbingTest, SearchPastCollision) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    table.insert("dave", 92);
    // Search for a key that was inserted after collisions
    ASSERT_NE(table.search("dave"), nullptr);
    EXPECT_EQ(*table.search("dave"), 92);
}

// ==================== TODO 12: remove() — tombstone (15 pts) =================

TEST(ProbingTest, RemoveExistingKey) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    EXPECT_TRUE(table.remove("alice"));
    EXPECT_EQ(table.size(), 1);
    EXPECT_EQ(table.search("alice"), nullptr);
}

TEST(ProbingTest, RemoveMissingKeyReturnsFalse) {
    ProbingHashTable table;
    table.insert("alice", 90);
    EXPECT_FALSE(table.remove("bob"));
    EXPECT_EQ(table.size(), 1);
}

TEST(ProbingTest, RemoveFromEmptyTable) {
    ProbingHashTable table;
    EXPECT_FALSE(table.remove("alice"));
}

TEST(ProbingTest, SearchWorksAfterRemove) {
    // The key test for tombstones: removing a key should not break
    // search for keys that were inserted after it
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);

    // Remove bob — search for carol should still work
    table.remove("bob");
    EXPECT_EQ(table.search("bob"), nullptr);
    ASSERT_NE(table.search("carol"), nullptr);
    EXPECT_EQ(*table.search("carol"), 78);
    ASSERT_NE(table.search("alice"), nullptr);
}

TEST(ProbingTest, InsertReusesTombstone) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.remove("alice");
    // Insert a new key — should be able to reuse the deleted slot
    table.insert("carol", 78);
    EXPECT_EQ(table.size(), 2);
    ASSERT_NE(table.search("carol"), nullptr);
    EXPECT_EQ(*table.search("carol"), 78);
}

TEST(ProbingTest, RemoveOnlyElement) {
    ProbingHashTable table;
    table.insert("alice", 90);
    EXPECT_TRUE(table.remove("alice"));
    EXPECT_EQ(table.size(), 0);
    EXPECT_TRUE(table.is_empty());
}

// ==================== TODO 13: resize() (15 pts) =============================

TEST(ProbingTest, ResizeTriggersOnHighLoadFactor) {
    ProbingHashTable table;  // capacity 7, max load 0.75
    table.insert("a", 1);
    table.insert("b", 2);
    table.insert("c", 3);
    table.insert("d", 4);
    table.insert("e", 5);
    table.insert("f", 6);  // 6/7 > 0.75 triggers resize before this insert
    EXPECT_GT(table.capacity(), 7);
    EXPECT_EQ(table.size(), 6);
}

TEST(ProbingTest, ResizePreservesAllEntries) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    table.insert("dave", 92);
    table.insert("eve", 88);
    table.insert("frank", 76);  // triggers resize

    ASSERT_NE(table.search("alice"), nullptr);
    EXPECT_EQ(*table.search("alice"), 90);
    ASSERT_NE(table.search("frank"), nullptr);
    EXPECT_EQ(*table.search("frank"), 76);
}

TEST(ProbingTest, ResizeClearsTombstones) {
    ProbingHashTable table;
    table.insert("alice", 90);
    table.insert("bob", 85);
    table.insert("carol", 78);
    table.remove("bob");  // creates a tombstone

    // Force resize by inserting enough entries
    // After remove: size=2, capacity=7. Need (size+1)/7 > 0.75 → size >= 5
    table.insert("dave", 92);
    table.insert("eve", 88);
    table.insert("frank", 76);
    table.insert("grace", 95);  // (5+1)/7 = 0.86 > 0.75 → triggers resize

    // After resize, tombstones should be cleared
    // and print should show no [deleted] entries
    std::string output = capture_probe_print(table);
    EXPECT_EQ(output.find("[deleted]"), std::string::npos);

    // All live entries should still be findable
    ASSERT_NE(table.search("alice"), nullptr);
    ASSERT_NE(table.search("carol"), nullptr);
    ASSERT_NE(table.search("dave"), nullptr);
    EXPECT_EQ(table.search("bob"), nullptr);  // was removed
}
