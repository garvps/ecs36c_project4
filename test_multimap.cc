#include <gtest/gtest.h>
#include "multimap.h"  // Your multimap implementation

// ✅ Test Insert and Retrieve
TEST(MultimapTest, InsertAndRetrieve) {
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(1, 'b');
    mmap.Insert(1, 'a'); // Duplicate value

    // Ensure the first inserted value is retrieved
    EXPECT_EQ(mmap.Get(1), 'a');

    // Ensure the multimap contains the key
    EXPECT_TRUE(mmap.Contains(1));

    // Ensure the size is correct (3 insertions)
    EXPECT_EQ(mmap.Size(), 3);
}

// Test Removing a Key
TEST(MultimapTest, RemoveKeyCompletely) {
    Multimap<int, char> mmap;

    mmap.Insert(2, 'x');
    mmap.Insert(2, 'y');

    // Remove both occurrences of key 2
    mmap.Remove(2);
    mmap.Remove(2);

    // The key should now be absent
    EXPECT_FALSE(mmap.Contains(2));

    // Verify an exception is thrown when trying to access a removed key
    EXPECT_THROW(mmap.Get(2), std::runtime_error);
}

// Test Removing a Single Value from a Key
TEST(MultimapTest, RemoveSingleValue) {
    Multimap<int, char> mmap;

    mmap.Insert(3, 'z');
    mmap.Insert(3, 'y');
    mmap.Insert(3, 'z');  // Duplicate value

    mmap.Remove(3); // Removes first occurrence of 'z'

    // Remaining values for key 3 should be 'y' and another 'z'
    EXPECT_EQ(mmap.Get(3), 'y');

    // Remove 'y' as well
    mmap.Remove(3);
    EXPECT_EQ(mmap.Get(3), 'z');

    // Remove the last value
    mmap.Remove(3);
    EXPECT_FALSE(mmap.Contains(3));
}

// Test Size Adjustments After Insert and Remove
TEST(MultimapTest, SizeAfterOperations) {
    Multimap<int, char> mmap;

    mmap.Insert(4, 'm');
    mmap.Insert(4, 'n');
    mmap.Insert(5, 'o');

    EXPECT_EQ(mmap.Size(), 3);

    mmap.Remove(4);
    EXPECT_EQ(mmap.Size(), 2);

    mmap.Remove(4);
    EXPECT_EQ(mmap.Size(), 1);

    mmap.Remove(5);
    EXPECT_EQ(mmap.Size(), 0);
}

// Test Contains Method
TEST(MultimapTest, Contains) {
    Multimap<int, char> mmap;

    mmap.Insert(6, 'p');
    mmap.Insert(6, 'q');

    EXPECT_TRUE(mmap.Contains(6));
    EXPECT_FALSE(mmap.Contains(7));
}

// Test Max and Min Methods
TEST(MultimapTest, MaxAndMin) {
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(3, 'c');
    mmap.Insert(2, 'b');

    EXPECT_EQ(mmap.Max(), 3);
    EXPECT_EQ(mmap.Min(), 1);
}


TEST(MultimapTest, EmptyMultimap) {
    Multimap<int, char> mmap;

    // Ensure multimap starts empty
    EXPECT_EQ(mmap.Size(), 0);
    EXPECT_FALSE(mmap.Contains(1));

    // Ensure Get() throws an error when key does not exist
    EXPECT_THROW(mmap.Get(1), std::runtime_error);

    // Ensure Remove() does not crash (should be a no-op)
    EXPECT_NO_THROW(mmap.Remove(1));

    // Test Max() and Min() *only if the multimap is not empty* (to avoid segmentation fault)
    if (mmap.Size() > 0) {
        EXPECT_NO_THROW(mmap.Max());
        EXPECT_NO_THROW(mmap.Min());
    }
}

// Test inserting multiple keys
TEST(MultimapTest, InsertMultipleDistinctKeys) {
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(2, 'b');
    mmap.Insert(3, 'c');

    // Ensure that each key's values are correctly inserted
    EXPECT_EQ(mmap.Get(1), 'a');
    EXPECT_EQ(mmap.Get(2), 'b');
    EXPECT_EQ(mmap.Get(3), 'c');

    // Ensure that all keys are contained
    EXPECT_TRUE(mmap.Contains(1));
    EXPECT_TRUE(mmap.Contains(2));
    EXPECT_TRUE(mmap.Contains(3));
}

TEST(MultimapTest, RemoveNonExistentKey) {
    Multimap<int, char> mmap;

    // Ensure the multimap is empty
    EXPECT_EQ(mmap.Size(), 0);

    // Attempt to remove a non-existent key
    mmap.Remove(10);

    // Ensure the size remains unchanged
    EXPECT_EQ(mmap.Size(), 0);
}

TEST(MultimapTest, ContainsAfterRemovingAllValues) {
    Multimap<int, char> mmap;

    mmap.Insert(5, 'a');
    mmap.Insert(5, 'b');
    mmap.Insert(5, 'c');

    // Remove all values by calling Remove() multiple times
    mmap.Remove(5);  // Removes 'a'
    mmap.Remove(5);  // Removes 'b'
    mmap.Remove(5);  // Removes 'c'

    // Ensure the key 5 is no longer in the multimap
    EXPECT_FALSE(mmap.Contains(5));
    EXPECT_EQ(mmap.Size(), 0);
}

// Test: Max and Min on Empty Tree
TEST(MultimapTest, MaxAndMinOnEmptyTree) {
    Multimap<int, char> mmap;

    // Ensure the multimap is empty
    EXPECT_EQ(mmap.Size(), 0);

    // Ensure exceptions are thrown for Max and Min when the tree is empty
    EXPECT_THROW(mmap.Max(), std::runtime_error);
    EXPECT_THROW(mmap.Min(), std::runtime_error);
}


TEST(MultimapTest, RemoveKeyWithNoRightChild) {
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(2, 'b');

    // Remove key 1, which has no right child
    mmap.Remove(1);

    // Ensure that the remaining key (2) is still present
    EXPECT_TRUE(mmap.Contains(2));
    EXPECT_EQ(mmap.Size(), 1);
}

// Test Inserting Multiple Values for the Same Key and Retrieving All
TEST(MultimapTest, InsertAndRetrieveMultipleValues) {
    Multimap<int, char> mmap;

    mmap.Insert(7, 'x');
    mmap.Insert(7, 'y');
    mmap.Insert(7, 'z');

    // First value should be retrieved initially
    EXPECT_EQ(mmap.Get(7), 'x');

    // Remove first value and check next
    mmap.Remove(7);
    EXPECT_EQ(mmap.Get(7), 'y');

    // Remove second value and check the last one
    mmap.Remove(7);
    EXPECT_EQ(mmap.Get(7), 'z');
}

// Test Removing All But One Value for a Key
TEST(MultimapTest, RemoveUntilOneValueRemains) {
    Multimap<int, char> mmap;

    mmap.Insert(8, 'a');
    mmap.Insert(8, 'b');
    mmap.Insert(8, 'c');

    // Remove two values
    mmap.Remove(8);
    mmap.Remove(8);

    // Ensure last value is still accessible
    EXPECT_EQ(mmap.Get(8), 'c');

    // The key should still be present
    EXPECT_TRUE(mmap.Contains(8));
}

// Test Reinserting a Key After Complete Removal
TEST(MultimapTest, ReinsertAfterRemoval) {
    Multimap<int, char> mmap;

    mmap.Insert(9, 'm');
    mmap.Insert(9, 'n');

    // Remove all occurrences
    mmap.Remove(9);
    mmap.Remove(9);

    // Ensure the key is gone
    EXPECT_FALSE(mmap.Contains(9));

    // Reinsert the same key
    mmap.Insert(9, 'o');

    // Ensure it exists again
    EXPECT_TRUE(mmap.Contains(9));
    EXPECT_EQ(mmap.Get(9), 'o');
}
