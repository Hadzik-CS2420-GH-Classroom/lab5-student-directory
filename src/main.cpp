#include <iostream>
#include <iomanip>

#include "ChainingHashTable.h"
#include "ProbingHashTable.h"

int main() {
    std::cout << "=== Module 5 Lab: Student Directory ===\n";

    // =========================================================================
    // Part 1: Chaining Hash Table
    // =========================================================================

    std::cout << "\n--- Part 1: Chaining Hash Table ---\n";

    ChainingHashTable chain_dir;
    std::cout << "Empty directory: size=" << chain_dir.size()
              << ", capacity=" << chain_dir.capacity() << '\n';

    // Register students (name -> student ID)
    std::cout << "\nRegistering students...\n";
    chain_dir.insert("Alice", 1001);
    std::cout << "  insert(\"Alice\", 1001)\n";
    chain_dir.insert("Bob", 1002);
    std::cout << "  insert(\"Bob\", 1002)\n";
    chain_dir.insert("Carol", 1003);
    std::cout << "  insert(\"Carol\", 1003)\n";
    chain_dir.insert("Dave", 1004);
    std::cout << "  insert(\"Dave\", 1004)\n";
    chain_dir.insert("Eve", 1005);
    std::cout << "  insert(\"Eve\", 1005)\n";

    std::cout << "\nDirectory contents:\n";
    chain_dir.print();

    // Look up students
    std::cout << "\nLooking up students...\n";
    int* result = chain_dir.search("Carol");
    std::cout << "  search(\"Carol\") = " << (result ? std::to_string(*result) : "not found") << '\n';
    result = chain_dir.search("Zara");
    std::cout << "  search(\"Zara\") = " << (result ? std::to_string(*result) : "not found") << '\n';

    // Update a student ID
    std::cout << "\nUpdating student ID...\n";
    chain_dir.insert("Alice", 9999);
    std::cout << "  insert(\"Alice\", 9999)\n";
    result = chain_dir.search("Alice");
    std::cout << "  search(\"Alice\") = " << (result ? std::to_string(*result) : "not found") << '\n';

    // Remove a student
    std::cout << "\nRemoving \"Bob\"...\n";
    chain_dir.remove("Bob");
    std::cout << "  Directory size: " << chain_dir.size() << '\n';

    // Resize test
    std::cout << "\nResize test (load factor trigger)...\n";
    std::cout << "  Before: capacity=" << chain_dir.capacity()
              << ", load_factor=" << std::fixed << std::setprecision(2)
              << chain_dir.load_factor() << '\n';

    chain_dir.insert("Frank", 1006);
    chain_dir.insert("Grace", 1007);
    chain_dir.insert("Heidi", 1008);
    chain_dir.insert("Ivan", 1009);

    std::cout << "  After more inserts: capacity=" << chain_dir.capacity()
              << ", load_factor=" << std::fixed << std::setprecision(2)
              << chain_dir.load_factor() << '\n';

    // =========================================================================
    // Part 2: Probing Hash Table
    // =========================================================================

    std::cout << "\n--- Part 2: Probing Hash Table ---\n";

    ProbingHashTable probe_dir;
    std::cout << "Empty directory: size=" << probe_dir.size()
              << ", capacity=" << probe_dir.capacity() << '\n';

    // Register students
    std::cout << "\nRegistering students...\n";
    probe_dir.insert("Frank", 2001);
    std::cout << "  insert(\"Frank\", 2001)\n";
    probe_dir.insert("Grace", 2002);
    std::cout << "  insert(\"Grace\", 2002)\n";
    probe_dir.insert("Heidi", 2003);
    std::cout << "  insert(\"Heidi\", 2003)\n";
    probe_dir.insert("Ivan", 2004);
    std::cout << "  insert(\"Ivan\", 2004)\n";

    // Look up students
    std::cout << "\nLooking up students...\n";
    result = probe_dir.search("Heidi");
    std::cout << "  search(\"Heidi\") = " << (result ? std::to_string(*result) : "not found") << '\n';
    result = probe_dir.search("Zara");
    std::cout << "  search(\"Zara\") = " << (result ? std::to_string(*result) : "not found") << '\n';

    // Remove with tombstone
    std::cout << "\nRemoving \"Grace\" (tombstone)...\n";
    probe_dir.remove("Grace");
    result = probe_dir.search("Heidi");
    std::cout << "  search(\"Heidi\") = " << (result ? std::to_string(*result) : "not found")
              << "  (still works past tombstone)\n";
    std::cout << "  size: " << probe_dir.size() << '\n';

    // Resize test
    std::cout << "\nResize test...\n";
    std::cout << "  Before: capacity=" << probe_dir.capacity() << '\n';

    probe_dir.insert("Judy", 2005);
    probe_dir.insert("Kevin", 2006);
    probe_dir.insert("Laura", 2007);

    std::cout << "  After more inserts: capacity=" << probe_dir.capacity()
              << "  (tombstones cleared)\n";

    std::cout << "\n=== Lab 5 Complete ===\n";

    return 0;
}
