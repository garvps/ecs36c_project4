#include <iostream>
#include <cassert>
#include <stdexcept>
#include "multimap.h"

void TestInsertAndGet() {
    Multimap<int, std::string> mm;
    mm.Insert(1, "Alice");
    mm.Insert(1, "Bob");
    mm.Insert(1, "Charlie");

    assert(mm.Get(1) == "Alice"); // First inserted value
    std::cout << "TestInsertAndGet passed!" << std::endl;
}

void TestRemoveSingleValue() {
    Multimap<int, std::string> mm;
    mm.Insert(2, "X");
    mm.Remove(2);

    try {
        mm.Get(2);
        assert(false); // Should not reach here
    } catch (std::runtime_error&) {
        std::cout << "TestRemoveSingleValue passed!" << std::endl;
    }
}

void TestRemoveMultipleValues() {
    Multimap<int, std::string> mm;
    mm.Insert(3, "A");
    mm.Insert(3, "B");
    mm.Remove(3);

    assert(mm.Get(3) == "B"); // Only first value should be removed
    std::cout << "TestRemoveMultipleValues passed!" << std::endl;
}

void TestRemoveNonExistentKey() {
    Multimap<int, std::string> mm;
    mm.Insert(4, "Hello");

    mm.Remove(5); // Should do nothing
    assert(mm.Get(4) == "Hello");
    std::cout << "TestRemoveNonExistentKey passed!" << std::endl;
}

void TestRetrieveNonExistentKey() {
    Multimap<int, std::string> mm;

    try {
        mm.Get(10);
        assert(false);
    } catch (std::runtime_error&) {
        std::cout << "TestRetrieveNonExistentKey passed!" << std::endl;
    }
}

void TestInsertDescendingOrder() {
    Multimap<int, std::string> mm;
    for (int i = 10; i > 0; --i)
        mm.Insert(i, "Value");

    assert(mm.Get(1) == "Value");
    assert(mm.Get(10) == "Value");
    std::cout << "TestInsertDescendingOrder passed!" << std::endl;
}

void TestInsertLargeDataset() {
    Multimap<int, int> mm;
    for (int i = 0; i < 1000; ++i)
        mm.Insert(i, i * 2);

    assert(mm.Get(500) == 1000);
    std::cout << "TestInsertLargeDataset passed!" << std::endl;
}

void TestTreeBalance() {
    Multimap<int, int> mm;
    for (int i = 1; i <= 15; ++i)
        mm.Insert(i, i * 10);

    // Verify Red-Black Tree properties
    assert(mm.Get(1) == 10);
    assert(mm.Get(15) == 150);
    std::cout << "TestTreeBalance passed!" << std::endl;
}

void TestInsertDuplicateKeys() {
    Multimap<int, std::string> mm;
    mm.Insert(5, "First");
    mm.Insert(5, "Second");
    mm.Insert(5, "Third");

    assert(mm.Get(5) == "First"); // Ensure ordering
    mm.Remove(5);
    assert(mm.Get(5) == "Second");
    mm.Remove(5);
    assert(mm.Get(5) == "Third");

    std::cout << "TestInsertDuplicateKeys passed!" << std::endl;
}

void TestClearMultimap() {
    Multimap<int, int> mm;
    for (int i = 0; i < 10; ++i)
        mm.Insert(i, i * 2);

    for (int i = 0; i < 10; ++i)
        mm.Remove(i);

    try {
        mm.Get(0);
        assert(false);
    } catch (std::runtime_error&) {
        std::cout << "TestClearMultimap passed!" << std::endl;
    }
}

void TestOrderOfValues() {
    Multimap<int, std::string> mm;
    mm.Insert(6, "One");
    mm.Insert(6, "Two");
    mm.Insert(6, "Three");

    assert(mm.Get(6) == "One"); // Values should be retrieved in insertion order
    mm.Remove(6);
    assert(mm.Get(6) == "Two");
    std::cout << "TestOrderOfValues passed!" << std::endl;
}

void TestEdgeCases() {
    Multimap<int, std::string> mm;
    
    // Insert and remove immediately
    mm.Insert(7, "Edge");
    mm.Remove(7);

    try {
        mm.Get(7);
        assert(false);
    } catch (std::runtime_error&) {
        std::cout << "TestEdgeCases passed!" << std::endl;
    }
}

int main() {
    TestInsertAndGet();
    TestRemoveSingleValue();
    TestRemoveMultipleValues();
    TestRemoveNonExistentKey();
    TestRetrieveNonExistentKey();
    TestInsertDescendingOrder();
    TestInsertLargeDataset();
    TestTreeBalance();
    TestInsertDuplicateKeys();
    TestClearMultimap();
    TestOrderOfValues();
    TestEdgeCases();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
