#include <gtest/gtest.h>
#include "multimap.h"

//inserting multiple values for the same key
TEST(MultimapTest, InsertAndRetrieve) { 
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(1, 'b');
    mmap.Insert(1, 'a');

    EXPECT_EQ(mmap.Get(1), 'a'); 
    EXPECT_TRUE(mmap.Contains(1)); 
    EXPECT_EQ(mmap.Size(), 3);  
}

// removing a key
TEST(MultimapTest, RemoveKeyCompletely) {
    Multimap<int, char> mmap;

    mmap.Insert(2, 'x');
    mmap.Insert(2, 'y');

    mmap.Remove(2);
    mmap.Remove(2);

    EXPECT_FALSE(mmap.Contains(2));  
    EXPECT_THROW(mmap.Get(2), std::runtime_error); 
}

// removing single value from a key w/ multiple values
TEST(MultimapTest, RemoveSingleValue) {
    Multimap<int, char> mmap;

    mmap.Insert(3, 'z');
    mmap.Insert(3, 'y');
    mmap.Insert(3, 'z');  

    mmap.Remove(3);
    EXPECT_EQ(mmap.Get(3), 'y');  

    mmap.Remove(3);  
    EXPECT_EQ(mmap.Get(3), 'z');  

    mmap.Remove(3);
    EXPECT_FALSE(mmap.Contains(3));  
}

// testing the size after insertions and removals
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

//testing if contains() works
TEST(MultimapTest, Contains) {
    Multimap<int, char> mmap;

    mmap.Insert(6, 'p');
    mmap.Insert(6, 'q');

    EXPECT_TRUE(mmap.Contains(6));
    EXPECT_FALSE(mmap.Contains(7));  
}

//testing if max and min works
TEST(MultimapTest, MaxAndMin) {
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(3, 'c');
    mmap.Insert(2, 'b');

    EXPECT_EQ(mmap.Max(), 3);
    EXPECT_EQ(mmap.Min(), 1);
}

// testing on empty multimap
TEST(MultimapTest, EmptyMultimap) {
    Multimap<int, char> mmap;

    EXPECT_EQ(mmap.Size(), 0);
    EXPECT_FALSE(mmap.Contains(1));
    EXPECT_THROW(mmap.Get(1), std::runtime_error);
    EXPECT_NO_THROW(mmap.Remove(1));

    if (mmap.Size() > 0) {  
        EXPECT_NO_THROW(mmap.Max());
        EXPECT_NO_THROW(mmap.Min());
    }
}

// inserting distinct keys
TEST(MultimapTest, InsertMultipleDistinctKeys) {
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(2, 'b');
    mmap.Insert(3, 'c');

    EXPECT_EQ(mmap.Get(1), 'a');
    EXPECT_EQ(mmap.Get(2), 'b');
    EXPECT_EQ(mmap.Get(3), 'c');

    EXPECT_TRUE(mmap.Contains(1));
    EXPECT_TRUE(mmap.Contains(2));
    EXPECT_TRUE(mmap.Contains(3));
}

// removing a non-existent key
TEST(MultimapTest, RemoveNonExistentKey) {
    Multimap<int, char> mmap;

    EXPECT_EQ(mmap.Size(), 0);
    mmap.Remove(10);
    EXPECT_EQ(mmap.Size(), 0);
}

// testing to see that a key is removed when all its values are deleted 
TEST(MultimapTest, ContainsAfterRemovingAllValues) {
    Multimap<int, char> mmap;

    mmap.Insert(5, 'a');
    mmap.Insert(5, 'b');
    mmap.Insert(5, 'c');

    mmap.Remove(5);
    mmap.Remove(5);
    mmap.Remove(5);

    EXPECT_FALSE(mmap.Contains(5));
    EXPECT_EQ(mmap.Size(), 0);
}

// max and min on empty multimap
TEST(MultimapTest, MaxAndMinOnEmptyTree) {
    Multimap<int, char> mmap;

    EXPECT_EQ(mmap.Size(), 0);
    EXPECT_THROW(mmap.Max(), std::runtime_error);
    EXPECT_THROW(mmap.Min(), std::runtime_error);
}

//removing a key with no right child
TEST(MultimapTest, RemoveKeyWithNoRightChild) {
    Multimap<int, char> mmap;

    mmap.Insert(1, 'a');
    mmap.Insert(2, 'b');

    mmap.Remove(1);
    EXPECT_TRUE(mmap.Contains(2));
    EXPECT_EQ(mmap.Size(), 1);
}

// inserting multiple values for the same key and then retrieving them
TEST(MultimapTest, InsertAndRetrieveMultipleValues) {
    Multimap<int, char> mmap;

    mmap.Insert(7, 'x');
    mmap.Insert(7, 'y');
    mmap.Insert(7, 'z');

    EXPECT_EQ(mmap.Get(7), 'x');

    mmap.Remove(7);
    EXPECT_EQ(mmap.Get(7), 'y');

    mmap.Remove(7);
    EXPECT_EQ(mmap.Get(7), 'z');
}

//removing values from a key until one value remains on it 
TEST(MultimapTest, RemoveUntilOneValueRemains) {
    Multimap<int, char> mmap;

    mmap.Insert(8, 'a');
    mmap.Insert(8, 'b');
    mmap.Insert(8, 'c');

    mmap.Remove(8);
    mmap.Remove(8);

    EXPECT_EQ(mmap.Get(8), 'c');
    EXPECT_TRUE(mmap.Contains(8));
}

// inserting a key back after removing all its values
TEST(MultimapTest, ReinsertAfterRemoval) {
    Multimap<int, char> mmap;

    mmap.Insert(9, 'm');
    mmap.Insert(9, 'n');

    mmap.Remove(9);
    mmap.Remove(9);

    EXPECT_FALSE(mmap.Contains(9));

    mmap.Insert(9, 'o');

    EXPECT_TRUE(mmap.Contains(9));
    EXPECT_EQ(mmap.Get(9), 'o');
}
